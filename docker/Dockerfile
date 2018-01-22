FROM kalka/steamcmd:latest
LABEL maintainer="Kalka <k@kalka.io>"

USER steam

ENV port=27015

ENV LD_LIBRARY_PATH=/opt/garrysmod/bin

USER root
COPY ./docker-entrypoint.sh /docker-entrypoint.sh
RUN chmod +x /docker-entrypoint.sh && \
    chown steam:steam /docker-entrypoint.sh && \
    mkdir /opt/garrysmod && \
    chown steam:steam /opt/garrysmod
USER steam

RUN /opt/steamcmd/steamcmd.sh +login anonymous +force_install_dir /opt +app_update 4020 validate +quit

VOLUME /opt/garrysmod/

EXPOSE ${port}/udp

ENTRYPOINT ["/docker-entrypoint.sh"]

CMD ["-game", "garrysmod", "+gamemode", "sandbox", "+map", "gm_flatgrass"]